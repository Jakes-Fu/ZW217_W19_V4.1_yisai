/******************************************************************************
**
**	Copyright (c) Newport Media Inc.  All rights reserved.
**
** 	Module Name:  nmidrv_atv.h
**		This module include the Nmi analog TV driver APIs.
**	Creator: Michael Jia
** 
*******************************************************************************/
#include "nmiport.h"
#include "nmiapi.h"
#include "nmicmn.h"
#include "sci_types.h"
#include "os_api.h"



/*****************************************************************************
*                                                                                                                                     *
*                                   Analog TV base function(power, play, scan...)                                 *
*                                                                                                                                     * 
******************************************************************************/



/******************************************************************************  
*  function:
*      atv_nmi600_poweron_init
*  para:
*       NULL
*description:
*      power on ,and initilize iic and  NMI600 chip
*
*******************************************************************************/
unsigned int atv_nmi600_poweron_init(void);


/******************************************************************************  
*  function:
*      atv_nmi600_poweroff_deinit
*  para:
*       NULL
*description:
*      power off ,and release iic
*
*******************************************************************************/
unsigned int atv_nmi600_poweroff_deinit(void);

/******************************************************************************  
*  function:
*      atv_nmi600_set_region
*  para:
*       region:  refer aRgnMap
*description:
*      set TV region
*
*******************************************************************************/
void  atv_nmi600_set_region(unsigned int region);


/******************************************************************************  
*  function:
*      atv_nmi600_get_total_channel_num
*  para:
*       region: input,  refer aRgnMap
         startID: output, start channel in this region
         endID: output, start channel in this region
          
*description:
*      get startID and endID in region
*
*******************************************************************************/
unsigned int atv_nmi600_get_total_channel_num(unsigned int region,unsigned int* startID,unsigned int* endID);


/******************************************************************************  
*  function:
*      atv_nmi600_fast_set_channel
*  para:
*       channelID: input
*description:
*      fast set TV channel, without wait for lock. if lock or not, return lock. it uses in play channel
*      this function doesn't fit scan.scan use function 'atv_nmi600_scan_one_channel' 
*
*******************************************************************************/
unsigned int atv_nmi600_fast_set_channel(unsigned int channelID);


/******************************************************************************  
*  function:
*       atv_nmi600_scan_one_channel
*  para:
*       channelID: input
*  description:
*       scan one channel, rerutn lock or not. 
*
*******************************************************************************/
unsigned int atv_nmi600_scan_one_channel(unsigned int channelID);



/******************************************************************************  
*  function:
*     atv_nmi600_get_rssi
*  para:
*       rssilevel: output (0-10, best rssi level is 10, worst level is 0)
*  description:
*       get current RSSI. Normally, if rssi level is below 2 or 3, it means low signal quality
*
*******************************************************************************/

void atv_nmi600_get_rssi(unsigned short *rssilevel);






/*****************************************************************************
*                                                                                                                                     *
*                                   Analog TV auxiliary function(volume,luma,UV...)                             *
*                                                                                                                                     * 
******************************************************************************/


/******************************************************************************  
*  function:
*      atv_nmi600_set_active
*  para:
*       active: input
*description:
*     set nmi600 active, select nmi600 as current image sensor device
*
*******************************************************************************/
void atv_nmi600_set_active(unsigned char  active);


/******************************************************************************  
*  function:
*      atv_nmi600_get_active
*  para:
*       active: input
*description:
*     get nmi600 active state
*
*******************************************************************************/
unsigned char  atv_nmi600_get_active(void);





/******************************************************************************  
*  function:
*     atv_nmi600_set_volume
*  para:
*       vol: input	0 to 12 (default: 8 ) 
	
*  description:
*       set nmi600 volume
*
*******************************************************************************/
void atv_nmi600_set_volume(unsigned int vol);


/******************************************************************************  
*  function:
*     atv_nmi600_set_mute
*  para:
*       vol: mute: 1-mute; 0-unmute
	
*  description:
*       set nmi600 audio mute
*
*******************************************************************************/
void  atv_nmi600_set_mute(unsigned char mute);



/******************************************************************************  
*  function:
*     atv_nmi600_set_scan_level
*  para:
*       scanlevel (input): 1 to 7 is the valid range 
*                                   the 7 is the most relax threshold and 1 is the most strigent (the default is 4).
*	
*  description:
*       set nmi600 scan level
*
*******************************************************************************/
void atv_nmi600_set_scan_level(unsigned int scanlevel);


/******************************************************************************  
*  function:
*     atv_nmi600_set_luma_gain
*  para:
*       lumagain: input  0-8(default is 4)	
	
*  description:
*       set luma gain. 
*
*******************************************************************************/
void atv_nmi600_set_luma_gain(unsigned char lumagain);




/******************************************************************************  
*  function:
*     atv_nmi600_set_luma_offset
*  para:
*       lumagain: input  -8~8(default is 0)	
	
*  description:
*       set luma offset. the value is bigger, the video will brighter. 
*
*******************************************************************************/
void atv_nmi600_set_luma_offset(int lumaoffset);


/******************************************************************************  
*  function:
*     atv_nmi600_set_uv_gain
*  para:
*       step: input	0 to 10 (default is 8)
	
*  description:
*       set uv gain. 
*
*******************************************************************************/
void atv_nmi600_set_uv_gain(unsigned int step);



/******************************************************************************  
*  function:
*     atv_nmi600_set_uv_snr_threshold
*  para:
*       high: input	high threshold
	  low:input      low threshold
*  description:
*       set snr threshold to adjust uv gain. Default high threshold is 20, low is 1. 
*
*******************************************************************************/
void atv_nmi600_set_uv_snr_threshold(double high, double low);


/******************************************************************************  
*  function:
*     atv_nmi600_set_uv_iirfactor
*  para:
*       factor (input): A value from 0 ~ 1(default is 1.0 )  
*                            0 means no change (fixed gain) and 1 has fastest chnage of color.
*	
*  description:
*       set the UV adjustment filter, make color change smooth
*
*******************************************************************************/
void atv_nmi600_set_uv_iirfactor(double factor);


/******************************************************************************  
*  function:
*     atv_nmi600_set_output_size,it can be set in run time
*  para:
*       sizetype:  	
						 bt320x240,
						 bt80x60,
						 bt176x144,
						 bt220x176,
						 bt240x180,
						 bt426x320,
						 bt360x240,
						 bt480x280,
						 bt480x360,

	
*  description:
*       set nmi600 bt output size
*
*******************************************************************************/
void atv_nmi600_set_output_size(unsigned int sizetype);



/******************************************************************************  
*  function:
*     atv_nmi600_set_fullscreen_mode
*  para:
*       isfull:  1-full screen mode; 0-smaller window mode
	
*  description:
*       set nmi600 mode(for test)
*
*******************************************************************************/
void atv_nmi600_set_fullscreen_mode(unsigned int isfull);




/******************************************************************************  
*  function:
*      atv_nmi600_power_save_for_camera
*  para:
*       NULL
*description:
*      power save mode, keep VDDIO on , NMI600 don't driver the YUV bus. 
*      this function should be called before camera power-on and initialization
*
*******************************************************************************/
void atv_nmi600_power_save_for_camera(void);
/*****************************************************************************
*                                                                                                                                     *
*                                  test function                                                                                 *
*                                                                                                                                     * 
******************************************************************************/



/******************************************************************************  
*  function:
*     atv_nmi600_show_pattern
*  para:
*       enable:  1-enable; 0-disable
	
*  description:
*       set nmi600 pattern output for test
*
*******************************************************************************/
void atv_nmi600_show_pattern(unsigned int enable);



/******************************************************************************  
*  function:
*     atv_nmi600_get_color_lock
*  para:
*       None
*  return: if lock, return 1; else return 0	
*  description:
*       set nmi600 pattern output for test
*
*******************************************************************************/
unsigned int atv_nmi600_get_color_lock(void);




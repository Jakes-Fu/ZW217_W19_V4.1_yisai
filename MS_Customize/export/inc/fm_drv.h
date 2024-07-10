/******************************************************************************
 ** File Name:      fm_drv.h                                                  *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           02/09/2006                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of  FM   *
 **                				                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 02/09/2006     Liangwen.Zhen     Create.                                  *
 ******************************************************************************/

#ifndef _FM_DRV_H_
#define _FM_DRV_H_

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
#include "sci_types.h"
typedef unsigned char		U8;
typedef unsigned char		bool;
typedef char				S8;
typedef unsigned short		U16;
typedef short				S16;
typedef unsigned int           U32;
typedef int                        S32;
typedef unsigned char	     UINT8;
typedef unsigned short	     UINT16;
typedef unsigned int	     UINT32;


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

//FM region
typedef enum
{
   	FM_REGION_NONE,		// None: use hardware limit
   	FM_REGION_MOBILE,		// FM Mobile (76MHz - 108 MHz, de-emp=50 Step100KHz)
   	FM_REGION_JAPAN,		// FM Japan  (76MHz - 90MHz, de-emp=50)
   	FM_REGION_US,		       // FM US (same as Europe, De-emp=75)
   	FM_REGION_JAPAN_WIDE,	// FM Japan wide: Same as Japan, with extra CH1(95.75) CH2(101.75) CH3(107.75)
   	FM_REGION_MAX
}FM_REGION_E;


//FM play channel
typedef enum
{
    FM_CHANNEL_HEADSET,            //play using headset 
    FM_CHANNEL_SPEAKER,             //play using Speaker

    FM_CHANNEL_MEX
}FM_VOL_CHANNEL_E;

//FM play volumn
typedef enum
{
 	FM_VOL_LEVEL_0,
    FM_VOL_LEVEL_1,
    FM_VOL_LEVEL_2,
    FM_VOL_LEVEL_3,
    FM_VOL_LEVEL_4,
    FM_VOL_LEVEL_5,
    FM_VOL_LEVEL_6,
    FM_VOL_LEVEL_7,
    FM_VOL_LEVEL_8,
    FM_VOL_LEVEL_9
} FM_VOL_LEVEL_E;


//FM Scan Level
typedef enum
{
   	FM_SCANLEVEL_LOW,
   	FM_SCANLEVEL_MID,
   	FM_SCANLEVEL_HIGH
}FM_SCANLEVEL_E;

      
//FM scan directory
typedef enum
{
   	FM_SCAN_DOWN,
   	FM_SCAN_UP
}FM_SCAN_DIR_E;


//FM scan mode
typedef enum
{
   	FM_SCAN_ONE,
   	FM_SCAN_ALL
}FM_SCAN_MODE_E;


//used by fm chip code
typedef enum
{
	FM_SEEK_TYPE_MANUAL = 0,
	FM_SEEK_TYPE_AUTO_ONE,
	FM_SEEK_TYPE_AUTO_ALL,
	FM_SEEK_TYPE_MAX
}FM_SEEK_TYPE_E;

//fm frequency range
typedef struct
{
    uint16   min_freq;                    //max frequency when fm is in one region
    uint16   max_freq;                     //max frequency when fm is in one region
}FM_FREQ_RANGE_T;   

//current FM info
typedef struct
{
	uint16 	freq;	
	uint8	signal_strength;
	uint8   is_stereo;
	
	FM_VOL_CHANNEL_E  fm_vol_channel ;
   	FM_VOL_LEVEL_E    fm_vol_level;
}FM_SINGLE_CHANNEL_INFO_T;

//all FM info when scanning all FM radios
typedef struct
{
	uint8 	freq_count;              //save effective FM radios count  when scanning all FM radios one time.    
	uint16  * auto_freq_tab_ptr;    //save effective FM radios' freq when scanning all FM radios on time.

}FM_ALL_CHANNEL_INFO_T;

typedef enum
{
   	FM_OUTPUT_HEADSET,
   	FM_OUTPUT_SPEAKER,
   	FM_OUTPUT_MUTE
}FM_OUTPUT_MODE_E;


typedef enum
{
    FM_SET_THRESHOLD_RSSI,
    FM_GET_RSSI,
    FM_CHANNEL_SPACE,
    FM_GET_STEREO,    //Add by luther.li 2012-12-01
    FM_CMD_MAX
}FM_CMD_INDEX_E;

//FM Operation set
typedef struct 
{
   uint32 	(*init)(void);
   void    	(*open)(void);
   void    	(*close)(void);
   
   uint32   (*playstart)(uint16 freq);             //play FM
   uint32   (*playstop)(void);             //stop FM
      
   uint32   (*manualseek)( 
   							uint16 					freq,
   	                       	FM_SCAN_DIR_E 			seek_dir, 
   	                       	uint8 					steps, 
   	                       	FM_SINGLE_CHANNEL_INFO_T *single_channel_info 
   	                      );    //manual seek
   uint32   (*autoseek)(
   							uint16 					freq, 
   				        	FM_SCAN_DIR_E 			seek_dir,
   				        	FM_SCAN_MODE_E			mode, 
   				        	FM_ALL_CHANNEL_INFO_T  *all_channel_info 
   				        );                //auto seek
  
   uint32   (*setscanlevel)(FM_SCANLEVEL_E level);     //set fm scan level
   uint32   (*setvol)(FM_VOL_LEVEL_E vol_level);                 //set fm play volume

   uint32   (*setfmregion)(FM_REGION_E  region);    //set fm region
   uint32   (*ioctl)(uint32 cmd, void *data);
   
}FM_OPERATION_T;

typedef enum
{
    FM_TYPE_INVALID,
    FM_TYPE_BUILDIN,
    FM_TYPE_EXTERNAL
}FM_TYPE_E;

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define FM_SUCCESS 				SCI_SUCCESS
#define FM_NOT_COMPLETE         SCI_WAIT_ERROR
#define FM_ERROR				SCI_ERROR


/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description:  set FM into sleep mode during phone power on
//  Note:  fm_ops[i]->close() should implement "entersleep" always         
/*****************************************************************************/
PUBLIC uint32 FM_Entersleep(void);

/*****************************************************************************/
//  Description:    This function is used to initialize FM function    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 FM_Init(void);

/*****************************************************************************/
//  Description:    This function is used to start FM playing    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 FM_PlayStart(uint16 freq);

/*****************************************************************************/
//  Description:    This function is used to stop FM playing    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 FM_PlayStop(void);

/*****************************************************************************/
//  Description:    This function is used to open FM function    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 FM_Open(void);

/*****************************************************************************/
//  Description:    This function is used to close FM function    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 FM_Close(void);


/*****************************************************************************/
//  Description:    This function is used to seek radio station manually.
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/

PUBLIC uint32 FM_ManualSeek (
              	uint16 						freq,
              	FM_SCAN_DIR_E 				seek_dir,
              	uint8 						steps, 
              	FM_SINGLE_CHANNEL_INFO_T *	single_channel_info
              );

/*****************************************************************************/
//  Description:    This function is used to seek radio station automatically.
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 FM_AutoSeek(
              	uint16 					freq,
				FM_SCAN_DIR_E			seek_dir,
				FM_SCAN_MODE_E			mode,			// 0 -> Search for next/previous station; 1 -> Auto scan
				FM_ALL_CHANNEL_INFO_T  *all_channel_info
			);

/*****************************************************************************/
//  Description:    This function is used to set FM volume.    
//  Author:         Liangwen.Zhen
//  Note:           
// 20110111: Will.Jiang: transfer "audio control" to <DRV_AUDIO>
/*****************************************************************************/
//PUBLIC uint32 FM_SetVol(FM_VOL_CHANNEL_E channel , FM_VOL_LEVEL_E  level);
/*****************************************************************************/
//  Description:    This function is used to Set scan level for fm.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 FM_SetScanLevel(FM_SCANLEVEL_E level);


/*****************************************************************************/
//  Description:    This function is used to Check if FM has init.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32  FM_SetRegion(FM_REGION_E  region);


/*****************************************************************************/
//  Description:    This function is used to Check if FM has init.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN FM_IsInit(void);

/*****************************************************************************/
//  Description:    This function is used to Check if FM has init.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32  FM_GetFreqRange(FM_FREQ_RANGE_T *fm_freq_range);

/*****************************************************************************/
//  Description:    This function is used to Check if FM has init.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32  FM_GetCurrentFMInfo (FM_SINGLE_CHANNEL_INFO_T  *single_channel_info);


/*****************************************************************************/
//  Description:    This function is used to get seek type
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC FM_SEEK_TYPE_E FM_GetSeekType(void);



/*****************************************************************************/
//  Description:    This function is used to command from api for FM.
//  Note:           
/*****************************************************************************/
PUBLIC uint32 FM_Ioctl(uint32 cmd, void *data);

/*****************************************************************************/
//  Description:  This function is used to set fm audio output path
//  Author:        
//  Note:           called by app: 
// 20110111: Will.Jiang: transfer "audio control" to <DRV_AUDIO>
/*****************************************************************************/
//PUBLIC uint32 FM_SetOutPutPath(FM_OUTPUT_MODE_E fm_outpath);

PUBLIC FM_TYPE_E FM_GetFmType(void);



   uint32 	FM_OPERATION_init(void);
   void    	FM_OPERATION_open(void);
   void    	FM_OPERATION_close(void);
   
   uint32   FM_OPERATION_playstart(uint16 freq);             //play FM
   uint32   FM_OPERATION_playstop(void);             //stop FM
      
   uint32   FM_OPERATION_manualseek( 
   							uint16 					freq,
   	                       	FM_SCAN_DIR_E 			seek_dir, 
   	                       	uint8 					steps, 
   	                       	FM_SINGLE_CHANNEL_INFO_T *single_channel_info 
   	                      );    //manual seek
   uint32   FM_OPERATION_autoseek(
   							uint16 					freq, 
   				        	FM_SCAN_DIR_E 			seek_dir,
   				        	FM_SCAN_MODE_E			mode, 
   				        	FM_ALL_CHANNEL_INFO_T  *all_channel_info 
   				        );                //auto seek
  
   uint32   FM_OPERATION_setscanlevel(FM_SCANLEVEL_E level);     //set fm scan level
   uint32   FM_OPERATION_setvol(FM_VOL_LEVEL_E vol_level);                 //set fm play volume

   uint32   FM_OPERATION_setfmregion(FM_REGION_E  region);    //set fm region
   uint32   FM_OPERATION_ioctl(uint32 cmd, void *data);

 
   void     FM_Set_VDDRF_IdleMode(void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    





#ifdef __cplusplus
}
#endif

#endif  // _FM_DRV_H_

// End of fm_drv.h

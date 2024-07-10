/******************************************************************************
 ** File Name:      fm_config.h                                                  *
 ** Author:        YueHuaZhong                                             *
 ** DATE:           05/29/2006                                                *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of FM.   *
 **                                                                           *
 ******************************************************************************/

#include "fm_drv.h"


//used by fm chip code
typedef enum
{
	FM_SEEK_TYPE_MANUAL = 0,
	FM_SEEK_TYPE_AUTO_ONE,
	FM_SEEK_TYPE_AUTO_ALL,
	FM_SEEK_TYPE_MAX
}FM_SEEK_TYPE_E;


typedef struct 
{
   uint32 (*init)(void);
   void    (*open)(void);
   void    (*close)(void);
   
   uint32   (*playstart)(uint16 freq);             //play FM
   uint32   (*playstop)();             //stop FM
      
   uint32   (*manualseek)( uint16 freq,uint8 seek_dir, uint8 steps, FM_SINGLE_CHANNEL_INFO_T *single_channel_info );    //manual seek
   uint32   (*autoseek)( uint16 freq, uint8 seek_dir,uint8 mode, FM_ALL_CHANNEL_INFO_T  *all_channel_info );                //auto seek
  
   uint32   (*setscanlevel)(FM_SCANLEVEL_E level);     //set fm scan level
   uint32   (*setvol)(FM_VOL_LEVEL_E vol_level);                 //set fm play volume

   uint32   (*setfmregion)(uint8 region);    //set fm region
   
}FM_OPERATION_T;



/*****************************************************************************/
//  Description:    This function is used to get FM operations
//  Author:        yuehz
//  Note:           
/*****************************************************************************/
PUBLIC FM_OPERATION_T * FM_GetOperations(void);


/*****************************************************************************/
//  Description:    This function is used to get seek type
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC FM_SEEK_TYPE_E FM_GetSeekType(void);


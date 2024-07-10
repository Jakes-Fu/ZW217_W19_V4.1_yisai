
/******************************************************************************
 ** File Name:    sensor_atv.h                                       *
 ** Author:       Tim.zhu                                               *
 ** DATE:         06/20/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 06/20/2010    Tim.zhu   Create.                                     *
 ******************************************************************************/
#ifndef _DC_ATV_H_
#define _DC_ATV_H_
/*----------------------------------------------------------------------------*
 **                          Dependencies                                     *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                          Compiler Flag                                    *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C" 
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/

typedef enum
{
    ATV_REGION_CHINA=0x00,
    ATV_REGION_HONGKONG,
    ATV_REGION_SHENZHEN,
    ATV_REGION_CAMBODIA,
    ATV_REGION_CANADA,
    ATV_REGION_CHILE,
    ATV_REGION_AFGHANISTAN,
    ATV_REGION_ARGENTINA,
    ATV_REGION_BRAZIL,
    ATV_REGION_INDONESIA,
    ATV_REGION_JAPAN,
    ATV_REGION_KOREA,
    ATV_REGION_LAOS,
    ATV_REGION_MALAYSIA,
    ATV_REGION_MEXICO,
    ATV_REGION_MYANMAR,
    ATV_REGION_PHILIPPINES,
    ATV_REGION_SINGAPORE,
    ATV_REGION_SOUTHAFRICA,
    ATV_REGION_TAIWAN,
    ATV_REGION_THAILAND,
    ATV_REGION_TURKEY,
    ATV_REGION_UAE,
    ATV_REGION_UK,
    ATV_REGION_USA,
    ATV_REGION_VENEZUELA,
    ATV_REGION_VIETNAM,
    ATV_REGION_WESTERNEUROPE,
    ATV_REGION_SPAIN,
    ATV_REGION_PORTUGAL,
    ATV_REGION_PAKISTAN,
    ATV_REGION_INDIA,
    ATV_REGION_AUSTRALIA,
    ATV_REGION_PARAGURY,
    ATV_REGION_URUGUAY,
    ATV_REGION_BULGARIA,
    ATV_REGION_CONGO,
    ATV_REGION_EGYPT,
    ATV_REGION_GERMANY,
    ATV_REGION_IRAQ,
    ATV_REGION_IRAN,
    ATV_REGION_HUNGRY,
    ATV_REGION_LIBYA,
    ATV_REGION_LEBANON,
    ATV_REGION_POLAND,
    ATV_REGION_ROMANIA,
    ATV_REGION_SANDIARABIA,
    ATV_REGION_TUNISIA,
    ATV_REGION_RUSSIA,
    ATV_REGION_MAX
}ATV_REGION_E;

typedef enum
{
	ATV_RSSI_0=0x00,
	ATV_RSSI_1,
	ATV_RSSI_2,
	ATV_RSSI_3,
	ATV_RSSI_4,
	ATV_RSSI_MAX
}ATV_RSSI_E;

typedef enum
{
    ATV_VOLUME_0=0x00,
    ATV_VOLUME_1,
    ATV_VOLUME_2,
    ATV_VOLUME_3,
    ATV_VOLUME_4,
    ATV_VOLUME_5,
    ATV_VOLUME_6,
    ATV_VOLUME_7,
    ATV_VOLUME_8,
    ATV_VOLUME_9,
    ATV_VOLUME_MAX
}ATV_VOLUME_E;

typedef enum
{
    ATV_CMD_CHIP_INIT=0x01,
    ATV_CMD_CHIP_SLEEP,
    ATV_CMD_SCAN_INIT,
    ATV_CMD_SCAN_CHN,
    ATV_CMD_SCAN_CHN_STOP,
    ATV_CMD_SET_CHN,
    ATV_CMD_DECT_REGION,
    ATV_CMD_SET_VOLUME,
    ATV_CMD_SET_REGION,
    ATV_CMD_GET_RSSI,
    ATV_CMD_GET_ALL_CHN_NUM,
    ATV_CMD_GET_IS_NTSC,
    ATV_CMD_GET_EMC,
    ATV_CMD_GET_INFO,
    ATV_CMD_CLOSE,
    ATV_CMD_MASK_ENABLE,
    ATV_CMD_MASK_DISABLE,
    ATV_CMD_MAX
}ATV_IOCTL_CMD_E;

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/


#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 


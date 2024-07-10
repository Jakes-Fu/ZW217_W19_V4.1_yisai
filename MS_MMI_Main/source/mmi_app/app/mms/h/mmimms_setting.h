/****************************************************************************
** File Name:      mmimms_setting.h                                        *
** Author:         aoke.hu                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        aoke.hu          create
** 
****************************************************************************/

#ifndef _MMIMMS_SETTING_H
#define _MMIMMS_SETTING_H

/**---------------------------------------------------------------------------*
 **                         Include Files                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "ffs.h"
#include "mmimms_export.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIMMS_SETTING_DEFAULT_INDEX    1
        
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
typedef enum
{    
    MMIMMS_SETTING_PRIORITY_HIGH,
    MMIMMS_SETTING_PRIORITY_NORMAL,
    MMIMMS_SETTING_PRIORITY_LOW,
    MMIMMS_SETTING_PRIORITY_MAX
}MMIMMS_SETTING_PRIORITY_E;

typedef enum
{    
    MMIMMS_SETTING_RETRIEVE_MANUAL,
    MMIMMS_SETTING_RETRIEVE_AUTO,
    MMIMMS_SETTING_RETRIEVE_MAX
}MMIMMS_SETTING_RETRIEVE_E;

typedef enum
{
    MMIMMS_SETTING_VALID_PERIOD_1_HOUR,
    MMIMMS_SETTING_VALID_PERIOD_12_HOUR,
    MMIMMS_SETTING_VALID_PERIOD_1_DAY,
    MMIMMS_SETTING_VALID_PERIOD_1_WEEK,
    MMIMMS_SETTING_VALID_PERIOD_LONGEST,
    MMIMMS_SETTING_VALID_PERIOD_MAX
}MMIMMS_SETTING_VALID_PERIOD_E;

typedef enum
{
    MMIMMS_PROTOCOL_WAP12,
    MMIMMS_PROTOCOL_WAP20,
    MMIMMS_PROTOCOL_MAX
}MMIMMS_PROTOCOL_SETTING_E;

//////////////////////////////////////////////////////////////////////////
// added by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
typedef enum _MMIMMS_SETTING_RETRIEVE_MODE_E
{
	MMIMMS_SETTING_RETRIEVE_MODE_DEFER,
	MMIMMS_SETTING_RETRIEVE_MODE_IMMEDIATE,
	MMIMMS_SETTING_RETRIEVE_MODE_DENY,
    MMIMMS_SETTING_RETRIEVE_MODE_MAX
}MMIMMS_SETTING_RETRIEVE_MODE_E;

typedef enum _MMIMMS_SETTING_TYPE_E
{
    MMIMMS_SETTING_TYPE_LOCAL,
    MMIMMS_SETTING_TYPE_ROAM,
    MMIMMS_SETTING_TYPE_MAX
}MMIMMS_SETTING_TYPE_E;
#endif

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Reset the mms setting to factory setting 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_NV_SETTING_T MMIMMS_ResetMMSFactorySetting(void);

/*****************************************************************************/
//     Description : get NDT nv setting
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMIMMS_GetNDTNvSetting(
                                       uint16 item_id, void* setting
                                       );

/*****************************************************************************/
//  Description : init mms setting 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_InitSetting(void);


/*****************************************************************************/
//  Description : Get delivery report 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_Setting_GetDeliveryReport(void);

/*****************************************************************************/
//  Description : Set delivery report 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetDeliveryReport(BOOLEAN is_send_delivery_rpt);

/*****************************************************************************/
//  Description : Get read report 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_Setting_GetReadReport(void);

/*****************************************************************************/
//  Description : Set read report 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetReadReport(BOOLEAN is_send_read_rpt);

/*****************************************************************************/
//  Description : Get anonymous 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_Setting_GetAnonymous(void);

/*****************************************************************************/
//  Description : Set anonymous 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetAnonymous(BOOLEAN is_anonymous);

/*****************************************************************************/
//  Description : Get play sound status 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_Setting_GetPlaysound(void);

/*****************************************************************************/
//  Description : Set delivery report 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetPlaysound(BOOLEAN is_play_sound);

/*****************************************************************************/
//  Description : Get advertising setting
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_Setting_GetAdvertising(void);

/*****************************************************************************/
//  Description : Set advertising setting 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetAdvertising(BOOLEAN is_advertisingallowable);

/*****************************************************************************/
//  Description : Get extract content status 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_Setting_GetExtractContent(void);

/*****************************************************************************/
//  Description : Set extract content status  
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetExtractContent(BOOLEAN is_extract_content);

/*****************************************************************************/
//  Description : Get priority setting 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_SETTING_PRIORITY_E MMIMMS_Setting_GetPriority(void);

/*****************************************************************************/
//  Description : Get priority setting 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetPriority(MMIMMS_SETTING_PRIORITY_E priority);

/*****************************************************************************/
//  Description : Get valid period 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_SETTING_VALID_PERIOD_E MMIMMS_Setting_GetValidPeriod(void);

/*****************************************************************************/
//  Description : Set valid period 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetValidPeriod(MMIMMS_SETTING_VALID_PERIOD_E valid_period);

/*****************************************************************************/
//  Description : Get recv delivery report status
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_Setting_GetRecvDeliveryReport(void);

/*****************************************************************************/
//  Description : Set recv delivery report status 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetRecvDeliveryReport(BOOLEAN is_need_delivery_rpt_recv);

/*****************************************************************************/
//  Description : Get prefer disk
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIMMS_Setting_GetPreferDisk(void);

/*****************************************************************************/
//  Description : Set prefer disk 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetPreferDisk(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
//  Description : Get mms net setting index 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIMMS_Setting_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Set mms net setting index  
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetNetSettingIndex(
                                               MN_DUAL_SYS_E dual_sys,
                                               uint8         index
                                               );

/*****************************************************************************/
//  Description : update mms net setting when the index list item is deleted  
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_UpdateNetSettingIndex(
                                                  uint8         index
                                                  );

//////////////////////////////////////////////////////////////////////////
// added by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
/*****************************************************************************/
//  Description : get retrieve mode from nv  
//  Global resource dependence : none
//  Author: feng.xiao
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_SETTING_RETRIEVE_MODE_E MMIMMS_Setting_GetRetrieveMode(MMIMMS_SETTING_TYPE_E type);
/*****************************************************************************/
//  Description : set retrieve mode 2 nv  
//  Global resource dependence : none
//  Author: feng.xiao
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetRetrieveMode(MMIMMS_SETTING_RETRIEVE_MODE_E retrieve_mode, 
                                           MMIMMS_SETTING_TYPE_E type);
/*****************************************************************************/
//  Description : get current retrieve mode
//  Global resource dependence : none
//  Author: feng.xiao
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_SETTING_RETRIEVE_MODE_E MMIMMS_Setting_GetCurRetrieveMode(MN_DUAL_SYS_E dual_sys);
#else
/*****************************************************************************/
//  Description : Set retrieve mode 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetRetrieveMode(MMIMMS_SETTING_RETRIEVE_E retrieve_mode);

/*****************************************************************************/
//  Description : Get retrieve mode 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_SETTING_RETRIEVE_E MMIMMS_Setting_GetRetrieveMode(void);

#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIMMS_SETTING_H*/

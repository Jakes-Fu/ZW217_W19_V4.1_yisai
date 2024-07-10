/******************************************************************************
 ** File Name:      drvapi_local.h                                          *
 ** Author:         Yi.Qiu                                                  *
 ** DATE:           02/21/2010                                              *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                            *
 ******************************************************************************

 ******************************************************************************
 **                 Edit History                                            *
 ** ------------------------------------------------------------------------*
 ** DATE            NAME             DESCRIPTION                            *
 ** 08/10/2010      Yi.Qiu          Create.                                 *
 ******************************************************************************/

#ifndef __DRVAPI_LOCAL_H__
#define __DRVAPI_LOCAL_H__

/* Clock API for local use */
PUBLIC uint32 CLOCK_Config (void);
PUBLIC uint32 CLOCK_SetSource (const char *ClkName, const char *SrcName);
PUBLIC const char *CLOCK_GetSource (const char *ClkName);
PUBLIC uint32 DEVICE_OpenClock (const char *DevName);
PUBLIC uint32 DEVICE_ClockClose (const char *DevName);
PUBLIC uint32 DEVICE_SetClock (const char *DevName, const char *SrcName, int Div);
PUBLIC const char *DEVICE_GetClock (const char *DevName);
PUBLIC const char *DEVICE_GetParent (const char *DevName);
PUBLIC uint32 DEVICE_Reset (const char *DevName);

/* ADC API for local use */
PUBLIC void ADC_SetScale (BOOLEAN scale);
PUBLIC void ADC_SetCs (uint32 source);
PUBLIC void ADC_OpenTPC (void);
PUBLIC void ADC_CloseTPC (void);
PUBLIC void ADC_ConfigTPC (uint8 x, uint8 y);

#endif

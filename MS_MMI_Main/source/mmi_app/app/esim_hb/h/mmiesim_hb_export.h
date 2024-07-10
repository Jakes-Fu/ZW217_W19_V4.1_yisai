/*****************************************************************************
** File Name:      mmiesim_hb_export.h                                             *
** Author:         yonghua.zhang                                             *
** Date:           07/15/2021                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:    This file is used to provide esim app api                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                     NAME             DESCRIPTION                     *
** 07/2020               yonghua.zhang            Create                     *
******************************************************************************/
#ifndef _MMIESIM_HB_EXPORT_H_
#define _MMIESIM_HB_EXPORT_H_

/*****************************************************************************/
//  Description : register ble callback for esim module
//  Parameter: [In]  NONE
//             [OUT] NONE
//             [Return] NONE
//  Author:yonghua.zhang
// Note:
/*****************************************************************************/
PUBLIC void MMIESIM_HB_BLEInit(void);

/*****************************************************************************/
//Description : EsimHB information window enter
//Global resource dependence : none
//Author: jichuan.zhang
//Note:none
/*****************************************************************************/
PUBLIC void MMI_Esim_HB_Information_Enter(void);

/*****************************************************************************/
//Description : ESIMHB modle init
//Global resource dependence : none
//Author: jichuan.zhang
//Note:none
/*****************************************************************************/
PUBLIC void MMIESIM_HB_InitModle(void);
#endif


/*****************************************************************************
** File Name:      watch_set_call_forwarding.h                               *
** Author:         xiaoju.cheng                                              *
** Date:           04/02/2021                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2021      xiaoju.cheng              Creat                              *
******************************************************************************/
#ifndef _WATCH_SET_CALLFORWARDING_H_
#define _WATCH_SET_CALLFORWARDING_H_

#include "ual_tele_common.h"

/*****************************************************************************/
//  Discription: enter setttings->call forwarding window                     *
//  Global resource dependence:                                              *
//  Author: xiaoju.cheng                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Watch_Settings_CallForwardingWin_Enter( void );
#ifdef UAL_SS_SUPPORT
/*****************************************************************************/
//  Discription: start call forwarding function                              *
//  Global resource dependence:                                              *
//  Author: xiaoju.cheng                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Watch_Settings_SS_Start_Operation(ual_sim_number_e     sim_num);

/*****************************************************************************/
//  Discription: start divert interrogate                                    *
//  Global resource dependence:                                              *
//  Author: xiaoju.cheng                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Watch_Settings_SS_Start_Interrogate(ual_sim_number_e     sim_num);
/*****************************************************************************/
//  Discription: start interrorate wait PS timer                             *
//  Global resource dependence:                                              *
//  Author: xiaoju.cheng                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Settings_SS_Interrogate_StartWaitPSTimer(void);

#endif
/*****************************************************************************/
//  Discription: start divert interrogate                                    *
//  Global resource dependence:                                              *
//  Author: xiaoju.cheng                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Watch_Settings_SS_Set_Operation_Status(BOOLEAN status);
/*****************************************************************************/
//  Discription: get ss operation status                                     *
//  Global resource dependence:                                              *
//  Author: xiaoju.cheng                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC BOOLEAN Watch_Settings_SS_Get_Operation_Status(void);
#endif

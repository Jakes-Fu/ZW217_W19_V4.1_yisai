/*****************************************************************************
** File Name:        audioplayer_demoapp_flow.h                                  *
** Author:           zirui.li                                                *
** Date:             12/13/2021                                              *
** Copyright:                                                                *
** Description:    This file is used to define audioplayer flow api          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 12/13/2021           zirui.li              Create                         *
******************************************************************************/
#ifndef _AUDIOPLAYER_DEMOAPP_FLOW_H_
#define _AUDIOPLAYER_DEMOAPP_FLOW_H_
#include "sci_types.h"
#include "mmk_type.h"
#ifdef UAL_AUDIOPLAYER_SUPPORT
#include "ual_audioplayer.h"
#endif

typedef BOOLEAN (*handle_events_callback)(MMI_WIN_ID_T win_id, uint32 msg_id, void* p_param, uint32 param_size);
/*****************************************************************************/
//  Description: Audioplayer test app enter
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC void AudioPlayer_Enter(void);

/*****************************************************************************/
//  Description: Audioplayer demoapp flow register callback
//  Parameter: [In] win_id  //view模块对应的win id
//             [In] p_callback
//             [Out] None
//             [Return] BOOLEAN
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AudioPlayer_Flow_Reg_Callback(MMI_WIN_ID_T win_id, handle_events_callback p_callback);

/*****************************************************************************/
//  Description: Audioplayer test destroy ual audioplayer service
//  Parameter: [In] None
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E Audioplayer_Flow_Destroy_Service(void);

/*****************************************************************************/
//  Description: Audioplayer test pause button callback
//  Parameter: [In] None
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E AudioPlayer_Flow_Pause(void);

/*****************************************************************************/
//  Description: Audioplayer test resume button callback
//  Parameter: [In] None
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E AudioPlayer_Flow_Resume(void);

/*****************************************************************************/
//  Description: Audioplayer test prev button callback
//  Parameter: [In] None
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E AudioPlayer_Flow_Prev(void);

/*****************************************************************************/
//  Description: Audioplayer test next button callback
//  Parameter: [In] None
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E AudioPlayer_Flow_Next(void);

#endif

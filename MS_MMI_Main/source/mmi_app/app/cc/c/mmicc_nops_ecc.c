/****************************************************************************
** File Name:      mmicc_nops_ecc.c                                                                                      *
** Author:                                                                                                                             *
** Date:              2020.5.14                                                                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                                  *
** Description:    This file is used to describe the window for cc                                              *
*****************************************************************************
**                         Important Edit History                          *
** --------------------------------------------------------------------------*
** DATE           NAME                         DESCRIPTION                             *
** 2020.5.14      yonnghua.zhang         Create
** 
****************************************************************************/

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "mmicc_export.h"
#include "mmicc_internal.h"
#include "mmi_appmsg.h"
#include "mmicc_id.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_text.h"
#include "mmi_common.h"
#include "window_parse.h"
#include "guitext.h"
#include "mmipub.h"
#include "mmiset_call_export.h"
/*---------------------------------------------------------------------------*/
/*                macro declaration                                          */
/*---------------------------------------------------------------------------*/
uint8 s_nops_ecc_telenum[CC_MAX_TELE_NUM_LEN] = {0};
/*****************************************************************************/
// 	Description : confirm with user, enable network for ECC or not
//	Global resource dependence : none
//	Author:yonghua.zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEnableNetworkForECCWinMsg(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );
LOCAL void SetFlyMode(void)
{
      uint32 i  = 0;
      BOOLEAN is_fly_mode_on = MMIAPISET_GetFlyMode();
      if (!is_fly_mode_on)
      {
          uint32 dual_sys =0;
          BOOLEAN is_ps_ready = TRUE;
          MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
          for (dual_sys = 0; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
          {
              if ((e_dualsys_setting & (0x01 << dual_sys))
                  && MMIAPIPHONE_GetSimExistedStatus(dual_sys))
              {
                  is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);
                  if (!is_ps_ready)
                  {
                      break;
                  }
              }
          }
          if (is_ps_ready)
          {
              is_fly_mode_on = TRUE;
              MMIAPISET_SetFlyMode(is_fly_mode_on);
              MMIAPIPHONE_DeactivePsAccordingToSetting();
              GUILABEL_SetIcon(WATCH_SHUTDOWN_FLYMODE_CTRL_ID, res_sys_btn_fly);
              MMK_SendMsg(WATCH_SHUTDOWN_WIN_ID, MSG_FULL_PAINT, PNULL);
          }
          else
          {
              SCI_TRACE_LOW("SetFlyMode open fail");
          }
      }
      else
      {
          BOOLEAN is_ps_deactive = TRUE;
          for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
          {
              if(!MMIAPIPHONE_GetIsPsDeactiveCnf((MN_DUAL_SYS_E)i))
              {
                  is_ps_deactive = FALSE;
                  break;
              }
          }
          if (is_ps_deactive)
          {
              is_fly_mode_on = FALSE;
              MMIAPISET_SetFlyMode(is_fly_mode_on);
              // 激活协议栈
              MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
              GUILABEL_SetIcon(WATCH_SHUTDOWN_FLYMODE_CTRL_ID, res_sys_btn_fly_off);
              MMK_SendMsg(WATCH_SHUTDOWN_WIN_ID, MSG_FULL_PAINT, PNULL);
          }
          else
          {
              SCI_TRACE_LOW("SetFlyMode close fail");
          }
      }
}

/*****************************************************************************/
// 	Description : confirm with user, enable network for ECC or not
//	Global resource dependence : none
//	Author:yonghua.zhang
//	Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleEnableNetworkForECCWinMsg(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    switch (msg_id)
    {
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            //clear ecc number
            SCI_MEMSET(s_nops_ecc_telenum, 0x0, sizeof(s_nops_ecc_telenum));
            SCI_TRACE_LOW("nops HandleEnableNetworkForECCWinMsg, reset s_nops_ecc_telenum");
            MMK_CloseWin( win_id );
            break;
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            MMICC_EnableNetworkForECC();
            MMK_CloseWin( win_id );
            break;
        default:
            recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
            break;
        }
    return recode;
}
/*****************************************************************************/
// 	Description : handle network start waiting window msg
//	Global resource dependence : none
//	Author:yonghua.zhang
//	Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleEnableNetworkWaitingMsg(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    MN_DUAL_SYS_E sim_num = MN_DUAL_SYS_1;
    CC_MAKE_CALL_INFO make_call_info_ptr = {0};

    switch (msg_id)
    {
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );
            break;
        case MSG_CC_NOPS_ECC_READY_SIM_ID_MSG:
            sim_num = *( ( MN_DUAL_SYS_E * )param );
            if(0 != strlen(s_nops_ecc_telenum))
            {
                SCI_MEMCPY(make_call_info_ptr.tele_num, s_nops_ecc_telenum, strlen(s_nops_ecc_telenum));
                make_call_info_ptr.tele_len = strlen(s_nops_ecc_telenum);
                SCI_TraceLow( "nops [%s] [%d] sim_num = %d", __FUNCTION__,__LINE__,sim_num );
                MMICC_ProcessMakeCall( sim_num, &make_call_info_ptr );
            }
            MMK_CloseWin( win_id );
            break;
       case MSG_CLOSE_WINDOW:
            //clear ecc number
            SCI_MEMSET(s_nops_ecc_telenum, 0x0, sizeof(s_nops_ecc_telenum));
            SCI_TRACE_LOW("nops HandleEnableNetworkForECCWinMsg, reset s_nops_ecc_telenum");			
	   break;
        default:
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
            break;
        }
    return recode;
}
/*****************************************************************************/
// 	Description : get whether exist available ps
//	Global resource dependence : none
//	Author:yonghua.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsNeedPowerOnPs(uint8 *tele_num, uint8 tele_len)
{
    BOOLEAN is_need_poweron_ps = FALSE;

    if(TRUE == MMIAPISET_GetFlyMode())
    {
         //飞行模式打开
        is_need_poweron_ps = TRUE;
    }
    else if((0 != MMIAPIPHONE_GetSimExistedNum(PNULL, 0) )&& (0 == MMIAPISET_GetMultiSysSettingNum(PNULL, 0)))
    {
        //插入的sim卡都被禁用
        is_need_poweron_ps = TRUE;
    }

    SCI_MEMSET(s_nops_ecc_telenum, 0, sizeof(s_nops_ecc_telenum));

    if(is_need_poweron_ps && (PNULL != tele_num) && (tele_len != 0))
    {
        SCI_MEMCPY(s_nops_ecc_telenum, tele_num, tele_len);
        SCI_TRACE_LOW("%s:%d , s_nops_ecc_telenum =%s", __FUNCTION__,__LINE__, s_nops_ecc_telenum);
    }

    return is_need_poweron_ps;
}
/*****************************************************************************/
// 	Description : 是否存在待播出的ecc号码
//	Global resource dependence : none
//	Author:yonghua.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistNoPSEccNumber(void)
{
    BOOLEAN is_nops_ecc_exist = FALSE;
    uint8 num_len = 0;

    num_len = strlen(s_nops_ecc_telenum);
    SCI_TRACE_LOW("%s:%d , num_len =%d", __FUNCTION__,__LINE__, num_len);

    if(num_len != 0)
    {
        is_nops_ecc_exist = TRUE;
    }
    SCI_TRACE_LOW("%s:%d , is_nops_ecc_exist =%d", __FUNCTION__,__LINE__, is_nops_ecc_exist);
    return is_nops_ecc_exist;
}

/*****************************************************************************/
// 	Description : enable network to implement ecc
//	Global resource dependence : none
//	Author:yonghua.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_EnableNetworkForECC(void)
{
    BOOLEAN is_flymode_on = FALSE;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint32 available_sim_num = 0;
    uint32 existed_sim_num = 0;
    BOOLEAN is_standby[MMI_DUAL_SYS_MAX] = {0};
#endif
    uint32 i = 0;
    MMI_STRING_T            wait_text = {0};
    BOOLEAN is_sim_poweron = FALSE;

    //open waiting window
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMICC_ENABLE_NETWORK_WAITING_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleEnableNetworkWaitingMsg);

    //获取sim卡是否已经power on
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
         if(TRUE == MMIAPIPHONE_GetIsSimPowerOn((MN_DUAL_SYS_E)i))
         {
              is_sim_poweron = TRUE;
              break;
          }
    }
    SCI_TRACE_LOW("%s:%d , is_sim_poweron =%d", __FUNCTION__,__LINE__, is_sim_poweron);

    if(!is_sim_poweron)
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        //双卡版本，需要在power sim卡之前，将所有的卡都设为待机状态
        MMISET_SetMultiSysSetting(MMISET_DAULSYS_ALL_SIM_ON);
#endif
        MMIAPISET_SetFlyMode(FALSE);
        MMIAPIPHONE_StartUpPsAccordingToSetting(TRUE, TRUE);
        return;
    }

    // 1.判断当前是否处于飞行模式，如果是，关闭飞行模式
    is_flymode_on = MMIAPISET_GetFlyMode();
    SCI_TRACE_LOW("%s:%d , is_flymode_on =%d", __FUNCTION__,__LINE__, is_flymode_on);

    if(TRUE == is_flymode_on)
    {
        ///MMIAPISET_SetFlyModeWholeFlow(FALSE);
        SetFlyMode();
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    {
        // 2.获取已插入sim卡的数目
        existed_sim_num = MMIAPIPHONE_GetSimExistedNum(PNULL, 0);
        SCI_TRACE_LOW("%s:%d , existed_sim_num =%d", __FUNCTION__,__LINE__, existed_sim_num);

        if(0 != existed_sim_num)
        {
            // 3.判断是否存在可用卡，卡存在且未被禁用
            available_sim_num = MMIAPISET_GetMultiSysSettingNum(PNULL, 0);
            SCI_TRACE_LOW("%s:%d , available_sim_num =%d", __FUNCTION__,__LINE__, available_sim_num);
            if(0 == available_sim_num)
            {
                for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
                {
                    if(TRUE == MMIAPIPHONE_GetSimExistedStatus((MN_DUAL_SYS_E)i))
                    {
                         // 4.找到已插入的sim卡，解禁用， power on ps
                        SCI_TRACE_LOW("%s:%d , power on ps of sim =%d", __FUNCTION__,__LINE__, i);
                        is_standby[(MN_DUAL_SYS_E)i] = TRUE;
                        MMIAPISET_SetDualSIMStandby(is_standby, (MN_DUAL_SYS_E)i);
                    }
                }
            }
        }
    }
#endif
}
/*****************************************************************************/
// 	Description : open query window, enable ps to do ecc or not?
//	Global resource dependence : none
//	Author:yonghua.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_OpenQueryWinForNoPSEcc(MMI_TEXT_ID_T text_id)
{
    MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKCANCEL,HandleEnableNetworkForECCWinMsg);    
}



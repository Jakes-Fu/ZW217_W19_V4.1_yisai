/*****************************************************************************
** File Name:  mmiphone_volte.h                                              *
** Author:                                                                   *
** Date:       03/30/2020                                                    *
** Copyright:                                                                *
** Description: This file is used to describe phone powner on/off            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2020        tingting.zheng   Create                                    *
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "mmiphone_export.h"
#include "mmiphone_internal.h"
#include "mmiset_export.h"
#include "mmi_nv.h"
#ifdef PRODUCT_DM
#include "mn_api_td.h"
#endif
#include "mn_api.h"
#include "ual_tele_radio.h"
/*---------------------------------------------------------------------------*/
/*                macro declaration                                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
LOCAL uint8 s_volte_mode_info = 0; //if bit 0 set to 1,sim0 volte function open,othersize close; if bit 1 set to 1,sim1 volte function open,othersize close
LOCAL BOOLEAN s_is_volte_ready[MMI_DUAL_SYS_MAX] = {FALSE};
LOCAL BOOLEAN s_is_volte_dereging[MMI_DUAL_SYS_MAX] = {FALSE};
LOCAL MMIPHONE_VOLTE_CONFIG_INFO s_volte_config_info[MMI_DUAL_SYS_MAX] = {0};
#define  MMI_IMS_TOGGLE_CONFIG_PARAMETER 
#ifdef MMI_VOLTE_SUPPORT
#ifdef MMI_IMS_TOGGLE_CONFIG_PARAMETER
#define MMI_IMS_TOGGLE_CONFIG_INGORE_MNC 0xffff //if mnc==0xffff, do not check mnc
#define IMS_TOGGLE_CONFIG_MAX_ITEM_NUM (ARR_SIZE(s_ims_toggle_config_table))
LOCAL const MMIPHONE_VOLTE_CONFIG_INFO s_ims_toggle_config_table[] =
{
    #include "ims_toggle_config_table.h"
};
#endif
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                   INTERNAL PUBLIC FUNCTION DEFINITION                     */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : deal with signal of APP_MN_MODEM_ACTIVATE_PDP_CONTEXT_IND
//  Parameter: [In] param:the param with the signal
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleActivatePdpCtxInd(DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

#ifdef MODEM_SUPPORT_SPRD
    APP_MN_GPRS_MODEM_ACTIVATED_PDP_IND_T pdp_ctx_ind = *((APP_MN_GPRS_MODEM_ACTIVATED_PDP_IND_T *)param);

#ifndef WIN32
    SCI_TRACE_LOW("MMIPHONE_HandleActivatePdpCtxInd:receive pdp_ctx_ind.status = %d, dual_sys = %d, cid = %d",
                    pdp_ctx_ind.status, pdp_ctx_ind.dual_sys,pdp_ctx_ind.cid);
    if ((1 == pdp_ctx_ind.status) && (11 == pdp_ctx_ind.cid))
    {
        result = MNPHONE_RegisterImsEx(pdp_ctx_ind.dual_sys);
        SCI_TRACE_LOW("MMIPHONE_HandleActivatePdpCtxInd: rigister result = %d",result);
    }
#endif
#endif

    return result;
}

/*****************************************************************************/
//  Description : deal with signal of APP_MN_IMS_STATE_IND
//  Parameter: [In] param:the param with the signal
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleImsStatusInd(DPARAM param)
{
    BOOLEAN bIsFound = FALSE;
    uint16 sim_ok = MN_DUAL_SYS_1;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMISET_DAULSYS_ONLY_SIM1_ON;
    APP_MN_IMS_STATE_IND_T ims_status_ind = *((APP_MN_IMS_STATE_IND_T *)param);
#ifdef MODEM_SUPPORT_RDA
    SCI_TraceLow("MMIPHONE_HandleImsStatusInd Ims state[%d]", ims_status_ind.nState);

    if ((MN_IMS_SERVICE_STATE_ACTIVE == ims_status_ind.nState) ||
    (MN_IMS_SERVICE_STATE_ROAMING == ims_status_ind.nState) ||
    (MN_IMS_SERVICE_STATE_REREGISTER_SUCCESS == ims_status_ind.nState))
    {
        MMIPHONE_SetVolteStatus(ims_status_ind.dual_sys,TRUE);
    }
    else
    {
        MMIPHONE_SetVolteStatus(ims_status_ind.dual_sys,FALSE);
    }
#endif
#ifdef MODEM_SUPPORT_SPRD
    APP_MN_IMS_STATE_IND_T ims_status_ind = *((APP_MN_IMS_STATE_IND_T *)param);

    SCI_TraceLow("MMIPHONE_HandleImsStatusInd s_is_volte_dereging[%d]=%d, reg_state=%d",
                 ims_status_ind.dual_sys, s_is_volte_dereging[ims_status_ind.dual_sys], ims_status_ind.reg_state);

    if ((MN_IMS_REG_STATE_REGISTERED == ims_status_ind.reg_state) ||
        (MN_IMS_REG_STATE_ROAMING == ims_status_ind.reg_state))
    {
        MMIPHONE_SetVolteStatus(ims_status_ind.dual_sys,TRUE);
    }
    else
    {
        MMIPHONE_SetVolteStatus(ims_status_ind.dual_sys,FALSE);
    }

    if ((s_is_volte_dereging[ims_status_ind.dual_sys]) && (MN_IMS_REG_STATE_INACTIVE == ims_status_ind.reg_state))
    {
        MNPHONE_PowerOffPsEx(ims_status_ind.dual_sys);
        s_is_volte_dereging[ims_status_ind.dual_sys] = FALSE;
    }
#endif
    MAIN_UpdateIdleSignalState();
    return result;
}

/*****************************************************************************/
//  Description : Deregister IMS
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] None
//             [Return] None
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_DeregisterIms(MN_DUAL_SYS_E dual_sys)
{
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    s_is_volte_dereging[dual_sys] = TRUE;
#ifdef MODEM_SUPPORT_SPRD

    MNPHONE_DeregisterImsEx(dual_sys);
#endif
    SCI_TraceLow("MMIPHONE_DeregisterIms: s_is_volte_dereging[%d] = %d",dual_sys,s_is_volte_dereging[dual_sys]);
    return;
}

/*****************************************************************************/
//  Description : Set Volte config info according with the config file
//  Parameter: [In] dual_sys: the value of the sim card
//             [In] hplmn : the value of plmn
//             [Out] None
//             [Return] None
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SetVolteConfigInfo(MN_DUAL_SYS_E dual_sys, MN_PLMN_T hplmn)
{
#ifdef MMI_IMS_TOGGLE_CONFIG_PARAMETER
    uint16 i = 0;
    uint16 len = IMS_TOGGLE_CONFIG_MAX_ITEM_NUM;
    const MMIPHONE_VOLTE_CONFIG_INFO *p_mmi_ims_toggle_item=PNULL;

    SCI_TraceLow("MMIPHONE_SetVolteConfigInfo: mcc[%d]=%d, mnc[%d]=%d",dual_sys,hplmn.mcc,dual_sys,hplmn.mnc);

    //mcc mnc check	
    
    for(i=0; i < len; i++)
    {
        p_mmi_ims_toggle_item = &(s_ims_toggle_config_table[i]);
        if(((p_mmi_ims_toggle_item->mcc == hplmn.mcc) && (p_mmi_ims_toggle_item->mnc == hplmn.mnc)) ||
    		((p_mmi_ims_toggle_item->mcc == hplmn.mcc) && (p_mmi_ims_toggle_item->mnc == MMI_IMS_TOGGLE_CONFIG_INGORE_MNC)))
        {
            s_volte_config_info[dual_sys].is_volte_icon_on = p_mmi_ims_toggle_item->is_volte_icon_on;
            s_volte_config_info[dual_sys].is_volte_default_on = p_mmi_ims_toggle_item->is_volte_default_on;
            s_volte_config_info[dual_sys].is_volte_set_display_on = p_mmi_ims_toggle_item->is_volte_set_display_on;
            s_volte_config_info[dual_sys].volte_icon_style = p_mmi_ims_toggle_item->volte_icon_style;
            SCI_TraceLow("MMIPHONE_SetVolteConfigInfo: set by config table, is_volte_icon_on=%d, is_volte_default_on=%d, is_volte_set_display_on=%d, volte_icon_style=%d",
            s_volte_config_info[dual_sys].is_volte_icon_on, s_volte_config_info[dual_sys].is_volte_default_on, 
            s_volte_config_info[dual_sys].is_volte_set_display_on, s_volte_config_info[dual_sys].volte_icon_style);
            return;
        }
        }
#endif
    //config table has no informatoin ,set by default value
    {
        s_volte_config_info[dual_sys].volte_icon_style = MMIPHONE_VOLTE_ICON_VOLTE;
        s_volte_config_info[dual_sys].is_volte_icon_on = TRUE;
        s_volte_config_info[dual_sys].is_volte_set_display_on = TRUE;
        s_volte_config_info[dual_sys].is_volte_default_on = TRUE;
        SCI_TraceLow("MMIPHONE_SetVolteConfigInfo: set by default value, volte_icon_style[%d] = %d, is_volte_icon_on = %d, is_volte_set_display_on = %d, is_volte_default_on = %d",
                    dual_sys,
                    s_volte_config_info[dual_sys].volte_icon_style,
                    s_volte_config_info[dual_sys].is_volte_icon_on,
                    s_volte_config_info[dual_sys].is_volte_set_display_on,
                    s_volte_config_info[dual_sys].is_volte_default_on);
    }
}

/*****************************************************************************/
//  Description : Init volte mode info according with the volte config info
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] None
//             [Return] None
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_InitVolteInfo(MN_DUAL_SYS_E dual_sys)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_VOLTE, &s_volte_mode_info, return_value);
    if ((MN_RETURN_SUCCESS != return_value) || (MMIAPIPHONE_IsSameSim(dual_sys) == FALSE))
    {
        //s_volte_mode_info = 0x3;
        if (TRUE == s_volte_config_info[dual_sys].is_volte_default_on)
        {
            s_volte_mode_info |=  (1 << dual_sys);
        }
        else
        {
            s_volte_mode_info &= ~(1 << (dual_sys));
        }

        MMINV_WRITE(MMINV_SET_VOLTE, &s_volte_mode_info);
    }

    if (s_volte_mode_info & (1 << dual_sys))
    {
        MMIAPIPHONE_SetVolteInfo(dual_sys, TRUE);
    }
    else
    {
        MMIAPIPHONE_SetVolteInfo(dual_sys, FALSE);
    }
    SCI_TraceLow("MMIPHONE_InitVolteInfo: volte_info[%d] =%d",dual_sys,s_volte_config_info[dual_sys].is_volte_default_on);

    return;
}

/*****************************************************************************/
//  Description : set Volte status
//  Parameter: [In] dual_sys: the value of the sim card
//             [In] is_volte_ready : TRUE :ready,FALSE :not ready;
//             [Out] None
//             [Return] None
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SetVolteStatus(MN_DUAL_SYS_E dual_sys , BOOLEAN is_volte_ready)
{
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }
    s_is_volte_ready[dual_sys] = is_volte_ready;

    //update status area
    //MMIAPICOM_StatusAreaSetVolteState(dual_sys,s_is_volte_ready[dual_sys]);
    
    SCI_TraceLow("MMIPHONE_SetVolteStatus: volte_status[%d] =%d",dual_sys,s_is_volte_ready[dual_sys]);
}

/*---------------------------------------------------------------------------*/
/*                   EXTERNAL PUBLIC FUNCTION DEFINITION                     */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get Volte status
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] None
//             [Return]TRUE :Open,FALSE :Close;
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetVolteStatus(MN_DUAL_SYS_E dual_sys)
{
    if (MMI_DUAL_SYS_MAX > dual_sys)
    {
        SCI_TraceLow("MMIAPIPHONE_GetVolteStatus volte_status[%d]=%d",dual_sys,s_is_volte_ready[dual_sys]);
        return s_is_volte_ready[dual_sys];
    }else
    {
        SCI_TraceLow("MMIAPIPHONE_GetVolteStatus: error dual_sys=%d",dual_sys);
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : Reset Ims status
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_ResetVolteInfo( void )
{
    uint16  dual_sys = MN_DUAL_SYS_1;

    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys ++)
    {
        if (TRUE == s_volte_config_info[dual_sys].is_volte_default_on)
        {
            MMIAPIPHONE_SetVolteInfo(dual_sys, TRUE);
        }
        else
        {
            MMIAPIPHONE_SetVolteInfo(dual_sys, FALSE);
        }
        SCI_TraceLow("MMIAPIPHONE_ResetVolteInfo: volte_info[%d] =%d",dual_sys,s_volte_config_info[dual_sys].is_volte_default_on);
    }
}

/*****************************************************************************/
//  Description : get volte info
//  Parameter: [In] None
//             [Out] None
//             [Return] 0:Open,1:Close;  Sim1:Bit0,Sim2:Bit1
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIPHONE_GetVolteInfo(void)
{
    SCI_TraceLow("MMIAPIPHONE_GetVolteInfo: s_volte_mode_info =%d",s_volte_mode_info);
    return s_volte_mode_info;
}

/*****************************************************************************/
//  Description : set volte info
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] volte_on:the status of volte
//             [Return] None
//  Author: feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIPHONE_SetVolteInfoNotToNV(MN_DUAL_SYS_E dual_sys,BOOLEAN volte_on)
{
    ual_tele_radio_result_e ret = UAL_TELE_RADIO_RES_SUCCESS;
    MMI_RESULT_E         result = MMI_RESULT_FALSE;

    SCI_TraceLow("MMIAPIPHONE_SetVolteInfo: dual_sys =%d,volte_on =%d",dual_sys,volte_on);
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return result;
    }

#ifndef WIN32
    ret = ual_tele_radio_set_volte(dual_sys, volte_on);
#endif

    if(UAL_TELE_RADIO_RES_SUCCESS == ret)
    {
        if(volte_on)
        {
            s_volte_mode_info |=  (1 << dual_sys);
        }
        else
        {
            s_volte_mode_info &= ~(1 << (dual_sys));
        }
        result = MMI_RESULT_TRUE;
    }

    SCI_TraceLow("MMIAPISET_SetVolteInfo: dual_sys = %d ,ret=%d,volte_on=%d",dual_sys, ret,volte_on);
    return result;

}

/*****************************************************************************/
//  Description : set volte info
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] volte_on:the status of volte
//             [Return] None
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIPHONE_SetVolteInfo(MN_DUAL_SYS_E dual_sys,BOOLEAN volte_on)
{
    ual_tele_radio_result_e ret = UAL_TELE_RADIO_RES_SUCCESS;
    MMI_RESULT_E         result = MMI_RESULT_FALSE;

    SCI_TraceLow("MMIAPIPHONE_SetVolteInfo: dual_sys =%d,volte_on =%d",dual_sys,volte_on);
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return result;
    }

#ifndef WIN32
    ret = ual_tele_radio_set_volte(dual_sys, volte_on);
#endif

    if(UAL_TELE_RADIO_RES_SUCCESS == ret)
    {
        if(volte_on)
        {
            s_volte_mode_info |=  (1 << dual_sys);
        }
        else
        {
            s_volte_mode_info &= ~(1 << (dual_sys));
        }

        MMINV_WRITE(MMINV_SET_VOLTE,&s_volte_mode_info);
        result = MMI_RESULT_TRUE;
    }
    SCI_TraceLow("MMIAPISET_SetVolteInfo: dual_sys = %d ,ret=%d,volte_on=%d",dual_sys, ret,volte_on);
    return result;

}

/*****************************************************************************/
//  Description : Get volte icon display style
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] None
//             [Return] volte icon display style
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC MMIPHONE_VOLTE_ICON_STYLE_E MMIAPIPHONE_GetVolteIconDisplayStyle(MN_DUAL_SYS_E dual_sys)
{
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return MMIPHONE_VOLTE_ICON_VOLTE;
    }
    SCI_TraceLow("MMIAPIPHONE_GetVolteIconDisplayStyle: volte_icon_style = %d",s_volte_config_info[dual_sys].volte_icon_style);
    return  s_volte_config_info[dual_sys].volte_icon_style;
}

/*****************************************************************************/
//  Description : Get volte icon display status
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] None
//             [Return] volte icon display status
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsVolteIconDisplay(MN_DUAL_SYS_E dual_sys)
{
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    SCI_TraceLow("MMIAPIPHONE_IsVolteIconDisplay: s_volte_is_icon_display[%d] =%d",s_volte_config_info[dual_sys].is_volte_icon_on,dual_sys);
    return s_volte_config_info[dual_sys].is_volte_icon_on;
}

/*****************************************************************************/
//  Description : Get volte set display status
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] None
//             [Return] volte menu display status
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsVolteSetDisplay(MN_DUAL_SYS_E dual_sys)
{
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    SCI_TraceLow("MMIAPIPHONE_IsVolteSetDisplay: is_volte_set_display_on[%d] =%d",dual_sys,s_volte_config_info[dual_sys].is_volte_set_display_on);
    return s_volte_config_info[dual_sys].is_volte_set_display_on;
}

/*****************************************************************************/
//  Description : Get volte set MMIAPIPHONE_VolteFactoryReset status
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_VolteFactoryReset()
{
    uint8 i = 0;
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX;i++)
    {
        MMIAPIPHONE_SetVolteInfo(i,TRUE);
    }
}

